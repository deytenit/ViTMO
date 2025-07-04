# 5. Файервол

## Две сети

### Подключение и адресация

```bash
sudo openvpn --config config.ovpn --setenv UV_NETWORK internalA
sudo ip addr add 10.43.67.1/24 dev tap0

sudo openvpn --config config.ovpn --setenv UV_NETWORK internalB
sudo ip addr add 10.219.196.1/24 dev tap1
```

### Включение пересылки пакетов

```bash
# Разрешить IPv4-форвардинг на уровне ядра
sudo sysctl -w net.ipv4.ip_forward=1

# Правило цепи пересылки по-умолчанию
sudo iptables -P FORWARD ACCEPT
```

## Ограничиваем подключения

### TCP только из A в B

```bash
sudo iptables
  -I FORWARD \ # Добавляет правило в начало цепи пересылочных пакетов
  -s 10.219.196.0/24 \ # Отправитель
  -d 10.43.67.0/24 \ # Адресат
  -p tcp \ # Протокол
  -m state --state NEW \ # Состояние соединения (NEW,RELATED,ESTABLISHED)
  -j DROP # Действие с запросом
```

**Расшифровка:**

- Для всех транзитных пакетов
- От клиентов подсети B
- До клиентов подсети A
- По протоколу tcp
- Новые соединения
- Отклонить

Аналогично все остальное.

### Ограничение UDP

```bash
sudo iptables -I FORWARD -s 10.43.67.0/24 -d 10.219.196.146/32 -p udp -m state --state NEW -m multiport ! --dports 3001,2745 \ # Кроме данных двух портов
	-j DROP
```

### Фильтр содержимого

```bash
sudo iptables -I FORWARD -p tcp \
	-m string --algo kmp --string "scores-59" \ # Ищем строку в содержимом используя алгоритм Кнута-Морриса-Пратта
  	-j DROP
```

## NAT

```bash
sudo iptables -t nat \ # Правило для таблицы NAT
	-A POSTROUTING \ # Очередь обработки перед отправкой пакета
  	-s 10.43.67.0/24 -o tap1 -j MASQUERADE

sudo iptables -t nat -A POSTROUTING -s 10.219.196.0/24 -o tap0 -j MASQUERADE
```

> MASQUERADE - динамически меняет отправителя на IP-адрес привязанный к исходящему интерфесу. Можно указывать адрес и вручную через `SNAT --to-source` 

По факту правила iptables хранятся в таблицах, в которых уже заданы цепи завязаные на определенные события. Можно создавать свои цепи, но и вызывать их нужно вручную из других.

Примеры таблиц:
- **filter**: базовая таблица для фильтрации
- **nat**: отвечает за трансляцию адресов
- **mangle**: изменение полей IP-заголовка

Примеры цепей:
- **FORWARD**: транзитные пакеты (не от хоста/для хоста)
- **INPUT**: пакеты для хоста
- **PREROUTING**: пакеты до маршрутизации
- **POSTROUTING**: пакеты перед выходом из файервола

### Настройка ICMP

```bash
sudo iptables -t mangle -I FORWARD -p icmp -j TTL --ttl-set 38

sudo iptables -t mangle -I FORWARD \
  -p icmp --icmp-type echo-request \ # Распространяется только на ping
  -m statistic --mode random --probability 0.25 \ # Cлучайный выбор пакетов для отправки
  -j DROP
```


### HTTP-доступ 

```bash
# Явно запрещаем http(s) трафик из подсети A
sudo iptables -I FORWARD -s 10.43.67.0/24 -p tcp -m multiport --dports 80,443 -j DROP

# Делаем перед правилом запрета исключение для nerc.itmo.ru
sudo iptables -I FORWARD -s 10.43.67.0/24 -d $(dig +short nerc.itmo.ru | head -n1) -p tcp -m multiport --dports 80,443 -j ACCEPT

# Обязательно транслируем внутренний адрес перед отправкой в интернет
sudo iptables -t nat -A POSTROUTING -s 10.43.67.0/24 -o enp0s9 -j MASQUERADE
```

> iptables работает с IP-адресами, поэтому, даже если ему передать домен в качестве аргумента, он его разрешит в момент создания правила, что может в будущем сыграть злую шутку.

### Завершение работы

Чистим таблички, откатываем изменения ядра:

```bash
sudo iptables -F
sudo iptables -t nat -F
sudo iptables -t mangle -F
sudo sysctl -w net.ipv4.ip_forward=0
```

