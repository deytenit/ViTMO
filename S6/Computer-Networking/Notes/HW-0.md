# 0. Настройка окружения

## Установка необходимых инструментов

### Создание виртуальных машин

#### Загрузка образа и конфигурация

```bash
mkdir -p ~/.local/share/qemu # Я предпочитаю хранить свои ВМ здесь
cd ~/.local/share/qemu
quickget ubuntu 24.10
```

Отредактируем файл конфигурации ubuntu-24.10.conf,
изменив параметры `display`, чтобы иметь более удобный интерфейс:

```bash
display="spice"
```

#### Создание второй виртуальной машины

```bash
cp -r ubuntu-24.10 ubuntu-24.10-2
cp -r ubuntu-24.10.conf ubuntu-24.10-2.conf
```

Отредактируем файл конфигурации ubuntu-24.10-2.conf,
изменив параметры `disk_img` и `vm_name`, чтобы они были уникальными:

```bash
disk_img="ubuntu-24.10-2/ubuntu-24.10-2.qcow2"
vm_name="ubuntu-24.10-2"
```

#### Запуск виртуальных машин

```bash
quickemu --vm ubuntu-24.10.conf
quickemu --vm ubuntu-24.10-2.conf
```

### Подготовка виртуальных машин

#### Установка zellij

Т.к. Ubuntu всеми обделённый дистрибутив,
когда я использую мультиплексер zellij,
поэтому можно воспользоваться [скриптом](https://gist.github.com/BaksiLi/ea2f505fdbe42349a5225390264c1f40).

```bash
curl -fsSL 'https://gist.githubusercontent.com/BaksiLi/ea2f505fdbe42349a5225390264c1f40/raw/ff71c02d9fb4f933dcf04c9fe5dbd863ef2c135e/install_zellij.sh' | sudo bash
```

> [Конфиг для zellij](https://github.com/deytenit/dotfiles/tree/linux) можно скачать аналогично,
> предворительно создав директорию конфигурации `~/.config/zellij`.

#### Установка OpenVPN

```bash
sudo apt update
sudo apt install openvpn
```

#### Подключение к VPN

> Через Spice просто копируем конфигурации ovpn

```bash
sudo openvpn --config config-linux.ovpn
```
