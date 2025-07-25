# 1. Протокол IP

## IP-адресация

### Определение сетевого интерфейса VPN

```bash
ip link
```

> После подключения к VPN на каждой ВМ, появляется новый сетевой интерфейс - `tap0`.

### Назначение IP-адресов

#### На первой ВМ

```bash
sudo ip addr add 10.123.0.23/24 dev tap0 # На первой ВМ
sudo ip addr add 10.123.0.24/24 dev tap0 # На второй ВМ
```

### Проверка соединения

```bash
ping 10.123.0.24 # На первой ВМ
```

### Эксперимент с произвольными IP-адресами

```bash
sudo ip addr add 6.6.6.1/24 dev tap0 # На первой ВМ
sudo ip addr add 6.6.6.2/24 dev tap0 # На второй ВМ
```

Проверим связь между ВМ:

```bash
ping 6.6.6.2  # С первой ВМ
```

> Так ВМ находятся в одной подсети, они "видят" друг друга.

### Проверка доступности в разных подсетях

Используя утилиту `ipcalc`,
можно в человеко-читаемом формате получить информцию о промежутке подсети.

```
sudo apt install ipcalc
ipcalc 10.123.0.17/28
```

#### Пример 1

- `10.123.0.17/28` -> `10.123.0.16`–`10.123.0.31`.
- `10.123.0.33/28` -> `10.123.0.32`–`10.123.0.47`.

> - [ ] Адреса находятся в разных подсетях,
поэтому ВМ не смогут напрямую обмениваться пакетами без маршрутизации.

#### Пример 2

- `192.168.77.5/24` -> `192.168.77.0`–`192.168.77.255`.
- `192.168.77.6/29` -> `192.168.77.0`–`192.168.77.7`.

> - [X] Адреса входят в оба диапазона подсетей,
поэтому ВМ смогут обмениваться пакетами.

#### Пример 3

- `172.19.21.5/23` -> `172.19.20.0`–`172.19.21.255`.
- `172.19.20.199/24` -> `172.19.20.0`–`172.19.20.255`.

> - [ ] Адреса не входят в оба диапазона подсетей,
поэтому не смогут обмениваться пакетами.

## IPv4

### Пинг неизвестного узла

Назначим первой ВМ предоставленный IP-адрес:

```bash
sudo ip addr add 10.216.211.83/24 dev tap0
```

Проверим доступность узла:

```bash
ping 10.216.211.129
```

### Определение MAC-адреса узла

```bash
ip neigh show 10.216.211.129
```

## IPv6

### Link-local адреса

#### Определение link-local адресов

Чтобы узнать link-local адреса наших ВМ:

```bash
ip -6 addr show tap0
```

#### Вычисление link-local адреса по MAC-адресу

Для вычисления link-local адреса IPv6 на основе MAC-адреса используется метод EUI-64. Так для MAC-адреса `02:B7:56:87:C4:6C`:

1. Разделяем MAC-адрес на две части: первые три октета (`02:B7:56`) и последние три октета (`87:C4:6C`).

2. Между этими двумя частями вставляем два байта `FF:FE`, чтобы получить 64-битный идентификатор интерфейса: `02:B7:56:FF:FE:87:C4:6C`.

3. Инвертируем бит U/L: этот бит является седьмым по счёту в первом октете. В данном случае первый октет — `02` (`b00000010`). Инвертируем седьмой бит, получая `b00000000` (`x00`).

4. **Формирование link-local адреса**: Добавляем префикс `FE80::/10` к полученному идентификатору интерфейса, получая полный link-local адрес: `FE80::B7:56FF:FE87:C46C`.

#### Проверка доступности узла по link-local адресу

```bash
ping6 -I tap0 fe80::b7:56ff:fe87:c46c
```

### Назначение глобальных IPv6-адресов

#### Назначение адресов

```bash
sudo ip -6 addr add fd33:96f1:852d:c552:8f70:2d39:58e0:16f/64 dev tap0 # На первой ВМ
sudo ip -6 addr add fd33:96f1:852d:c552:8f70:2d39:58e0:17/64 dev tap0 # На второй ВМ
```

#### Проверка соединения между ВМ

```bash
ping6 fd33:96f1:852d:c552:8f70:2d39:58e0:17/64
```

#### Пинг удалённого узла

```bash
ping6 fd33:96f1:852d:c552:ba3f:f11b:923f:4c92
```

#### Определение MAC-адреса удалённого узла

```bash
ip -6 neigh show fd33:96f1:852d:c552:ba3f:f11b:923f:4c92
```

### Определение link-local адресов всех узлов в сети

Отправим пинг на мультикаст, получим ответы с соответствующими адресами от всех ВМ в сети:

```bash
ping6 -I tap0 ff02::1
```

## Определение MTU

Простенький бинпоиск тыкает узел с разыми значениями размера пакета.
Нужно учитывать, что здесь задается именно размер тела пакета ICMP - не включая ICMP и IP хедера.

```
#!/bin/bash

ADDR="fd33:96f1:852d:c552:ba3f:f11b:923f:4c92"

l=0
r=1500
while [ $l -lt $r ]; do
  m=$(( (l+r+1)/2 ))
  echo "$m"
  ping6 -c1 -s $m $ADDR &>/dev/null && l=$m || r=$(( m-1 ))
done

echo "$((l + 40 + 8))" # IPv6 header + ICMP header 
```
