# 3. DHCP-сервер

## Сервер

### Настройка сети

Выдаем себе IP-адреса, как шлюзу по-умолчанию.

```bash
sudo ip addr add 10.209.34.1/24 dev tap0
sudo ip -6 addr add fdb0:b337:c1c9:f39d::1/64 dev tap0
```

### Установка dnsmasq

```bash
sudo apt update && sudo apt install dnsmasq
```

> dnsmasq - это легковесный DNS и DHCP сервер.
>
> Он используется в Pi-hole, что является оберткой над dnsmasq для DNS фильтрации. Его, в свою очередь, я довольно долгое время использую в личной инфрастуктуре.

Выключаем стандартный DNS сервер, т.к. он занимает порт 53

```bash
sudo systemctl stop systemd-resovled
```

### Настройка dnsmasq

В `/etc/dnsmasq.conf`:

```
# Интерфейс VPN
interface=tap0

# Форсируем dnsmasq, как DHCP сервер
dhcp-authoritative

# Пул IPv4-адресов из сети 10.209.34.0/24 с временем жизни адреса - сутки
dhcp-range=10.209.34.2,10.209.34.254,255.255.255.0,24h

# Шлюз по умолчанию
dhcp-option=option:router,10.209.34.1

# Включаем RA и устанавливаем бит A для SLAAC
dhcp-range=fdb0:b337:c1c9:f39d::,ra-only,slaac,24h

# Включаем логирование DHCP-запросов
log-dhcp

# Направлем логи в файл
log-facility=/var/log/dnsmasq.log
```

Создаем файл для логов:

```bash
sudo touch /var/log/dnsmasq.log
sudo chown dnsmasq:adm /var/log/dnsmasq.log # Меняем владельца файла на dnsmasq в группе adm, для корректности
```

Перезапускаем dnsmasq:

```bash
sudo systemctl restart dnsmasq
```

> После этого клиенты начнут получать IP-адреса.

## Проверка

### Получение адресов на клиенте

```bash
sudo dhclient tap0
ip addr show tap0
```

> Убеждаемся, что получен IPv4-адрес из диапазона `10.209.34.0/24` и IPv6-адрес с префиксом `fdb0:b337:c1c9:f39d::/64`.

## Логи и MAC-адрес

### Просмотр логов DHCP

```bash
sudo cat /var/log/dnsmasq.log | grep 'DHCPREQUEST'
```

> Найти запись вида:

```bash
DHCPREQUEST(tap0) 10.209.34.x 02:02:07:bb:9a:b1
```

> Где `02:02:07:bb:9a:b1` — MAC-адрес клиента.

## Завершение

Выключаем dnsmasq

```bash
sudo systemctl disable --now dnsmasq
```

Т.к. мы только остановили resolved, то, при следующем запуске ВМ, он восстановит свою работу.
