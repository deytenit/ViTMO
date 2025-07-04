# 4. Протокол DNS

## Структура DNS‑серверов

### Получение IP‑адреса

Выполним запрос к любому рекурсивному резолверу (I use unbound btw):

```bash
dig +short nerc.itmo.ru
```

### Получение IP-адреса без рекурсивного сервера

1. Делаем запрос к корневому серверу без рекурсивности:
```bash
dig @a.root-servers.net nerc.itmo.ru A +norecurse
```

2. Получив из секции AUTHORITY список авторитетный для зоны `ru.` серверов и из секции ADDITIONAL их адреса, отправляем запрос одному из них:
```bash
dig @193.232.128.6 nerc.itmo.ru A +norecurse # a.dns.ripn.net
```

3. Из ответа берем авторитетный сервер для зоны `itmo.ru`, чтобы уже у него узнать ответ из секции ANSWER:
```bash
dig @77.234.194.2 nerc.itmo.ru A +norecurse # ns.itmo.RU
```

## DNS‑сервер

### Настройка сети

Выдаем себе IP-адреса, как шлюзу по-умолчанию.

```bash
sudo ip addr add 10.52.1.1/24 dev tap0 
```

### Установка Unbound

```bash
sudo apt update && sudo apt install unbound
sudo systemctl enable --now unbound.service
```

### Настройка Unbound

В файле `/etc/unbound/unbound.conf`:

```yaml
server:
  interface: 127.0.0.1@53531 # Локальный сервер для "обычных" доменов
  access-control: 0.0.0.0/24 allow # Обслуживаем всех в локальной сети
  do-ip4: yes # Обслуживаем IPv4
  do-ip6: no # Не обслуживаем IPv6
```

Перезагружаем:

```bash
sudo unbound-checkconf && sudo systemctl restart unbound
```

### Трансфер

> Чтобы решить последнее задание требуется сервер выполняющий функции _Авторитетного DNS-сервера_, чем Unbound в общем-то не является. Unbound - это производительный рекурсивный кеширующий DNS-сервер, который лучше с большим комфортом разместить у себя в сети, где только это и требуется.

Ребята из NLnet Labs предлагают "папу-unbound" - NSD.

### Установка NSD

```bash
sudo apt update && sudo apt install nsd
sudo systemctl enable --now nsd
```

### Настройка NSD

В файле `/etc/nsd/nsd.conf`:

```
server:
    ip-address: 127.0.0.1@53530 # Локальный сервер запросов авторитетных зоне `4iy9ii82cg.localnetwork`

# Объявление зоны для трансфера
zone:
    name: "4iy9ii82cg.localnetwork"
    zonefile: "/etc/nsd/zones/4iy9ii82cg.localnetwork.zone" # Файл зоны
   	notify: 10.52.1.2 NOKEY # Уведомляем дублирующий сервер
    provide-xfr: 127.0.0.1 NOKEY # Предоставляем свободный доступ AXFR
```

Создаем файл зоны в `/etc/nsd/zones/4iy9ii82cg.localnetwork.zone` - тут все элементарнее, чем в Unbound:

```
$TTL 3600
@   IN SOA  ns.4iy9ii82cg.localnetwork. noreply.4iy9ii82cg.localnetwork. (
		2025042300  ; serial YYYYMMDDNN
		1800        ; refresh (30m)
		900         ; retry (15m)
		604800      ; expire (1w)
		86400       ; minimum (1d)
	)
    IN NS   ns.4iy9ii82cg.localnetwork.

@   IN A     10.52.1.140
@   IN AAAA  fd44:1337::4881:3286:1b63:f85b
@   IN MX 10 mx.example.org.

ns    IN A     10.52.1.1
ofy5n IN AAAA fd44:1337::4a22:958b:b472:ecb7
```

> **$TTL:** базовое время жизни ответа для записей

> **SOA:** мастер-сервер, электропочта администратора, серийный номер зоны, интервал проверки обновлений дублирующими серверами, интервал повторной попытки связи с мастер-сервером, время прекращения обслуживания зоны при отсутствии связи с мастер-сервером, минимальное время кэширования отрицательных ответов

> **S IN X \<P\> С:** запись типа X с содержанием C по поддомену S (@ для корня) с приоритетом P

Перезагружаем:

```bash
nsd-checkconf /etc/nsd/nsd.conf && sudo systemctl restart nsd
```

### Установка DnsList

> К сожалению, у меня не получилось провести AXFR запросы через Unbound в NSD (если это вообще возможно...). Хотя это должен быть шаблонный подход через `stub-zone`. Такое ощущение, что Unbound вообще их сразу отрезает, а информации по этому поводу я никак не нашел. В отличии от BIND9 тут есть четкая специализация между DNS серверами, из-за чего здесь такие костыли :)
>
> _**Может их в принципе не положено мешать так?**_

```bash
sudo apt update && sudo apt install unbound
```

### Настройка DnsList

Идея в том, чтобы все, что связано с нашей зоной отдать NSD, а остальное Unbound.

В `/etc/dnsdist/dnsdist.lua`:

```lua
addLocal("10.52.1.1:53") -- Слушаем на интерфейсе VPN

newServer({address="127.0.0.1:53530", name="nsd", pool="nsd"}) -- Локальный NSD
newServer({address="127.0.0.1:53531", name="unbound", pool="unbound"}) -- Локальный Unbound

-- Все уведомления в nsd
addAction(
  OrRule({
    OpcodeRule(DNSOpcode.Notify)
  }),
  PoolAction("nsd")
)

-- Соответствующие SOA в NSD
addAction(
  AndRule({
    QTypeRule(DNSQType.SOA),
    QNameSuffixRule("4iy9ii82cg.localnetwork.")
  }),
  PoolAction("nsd")
)

-- Соответствующие AXFR в NSD
addAction(
  AndRule({
    QTypeRule(DNSQType.AXFR),
    QNameSuffixRule("4iy9ii82cg.localnetwork.")
  }),
  PoolAction("nsd")
)

-- Соответствующие домены в NSD
addAction(
  QNameSuffixRule("4iy9ii82cg.localnetwork.")
  PoolAction("nsd")
)

-- Все остальное в Unbound
addAction(
  AllRule(),
  PoolAction("unbound")
)
```

Запускаем в терминале DnsList сервер:

```bash
sudo dnsdist --config=/etc/dnsdist/dnsdist.lua -v
```

### Проверка

```
dig @10.52.1.1 4iy9ii82cg.localnetwork AXFR +noall +answer +tcp
dig @10.52.1.1 4iy9ii82cg.localnetwork A AAAA MX NS SOA
dig @10.52.1.1 ofy5n.4iy9ii82cg.localnetwork AAAA
```

Чтобы включить наш сервер в системе, нужно познакомить его, в моем случае, c systemd-resolved.

В `/etc/systemd/resolved.conf`:

```
DNS=10.52.1.1 1.1.1.1
```

Успешно открываем страницу в браузере.

### Завершение работы

```
sudo systemctl disable --now nsd
sudo systemctl disable --now unbound
```
