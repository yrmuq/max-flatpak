# MAX Flatpak (неофициальная сборка)

[![Build Status](https://github.com/yrmuq/max-flatpak/actions/workflows/build.yml/badge.svg)](https://github.com/yrmuq/max-flatpak/actions/workflows/build.yml)
[![Latest Release](https://img.shields.io/github/v/release/yrmuq/max-flatpak)](https://github.com/yrmuq/max-flatpak/releases/latest)

## ⚠️ ВАЖНОЕ ПРЕДУПРЕЖДЕНИЕ

Это **НЕОФИЦИАЛЬНЫЙ** пакет мессенджера MAX, не связанный с разработчиками VK или создателями оригинального приложения.

Репозиторий берёт официальный Linux-rpm с `download.max.ru` и кладёт его целиком в Flatpak. При установке на компьютер больше ничего не скачивается.

---

## Особенности

- Нет доступа ко всей домашней папке. Файлы только через системный диалог.
- Окно через X11. На Wayland это XWayland: в официальном rpm нет плагина окна Wayland.
- Звонки через PipeWire.
- Уведомления и трей.

---

## Установка с автообновлением

Скачайте `ru.max.Unofficial.flatpakref` со страницы [Releases](https://github.com/yrmuq/max-flatpak/releases/latest). Это маленький файл с адресом. Сам клиент Flatpak скачает с сайта.

```bash
flatpak remote-delete --user max-repo || true
flatpak uninstall --user ru.max.Unofficial || true
flatpak install --user ru.max.Unofficial.flatpakref
flatpak run ru.max.Unofficial
```

Дальше обновление:

```bash
flatpak update
```

---

## Установка одним файлом

Скачайте `MAX.flatpak` с той же страницы. Это полный пакет без автообновления.

```bash
flatpak uninstall --user ru.max.Unofficial || true
flatpak install --user MAX.flatpak
flatpak run ru.max.Unofficial
```

Новую версию так же ставят поверх этой командой.

---

## Удаление

```bash
flatpak uninstall --user ru.max.Unofficial
flatpak remote-delete --user max-unofficial || true
```

---

## Технические детали

- **App ID**: `ru.max.Unofficial`
- **Runtime**: `org.freedesktop.Platform 26.08`
- **Источник**: официальный rpm `https://download.max.ru/linux/rpm/el/9/x86_64/`
- Программа лежит в `/app/opt/max` и запускается как `/app/opt/max/bin/max`
- Домашняя папка закрыта. Есть сеть, звук, камера, PipeWire, уведомления и трей.
- GitHub Actions раз в сутки в 15:00 UTC проверяет новый rpm. Если версия новая, собирает полный `MAX.flatpak`, подписывает каталог обновлений и кладёт `ru.max.Unofficial.flatpakref` в Releases.

Вопросы о самом мессенджере — в поддержку VK. Этот репозиторий отвечает только за упаковку.

- **Упаковка и скрипты**: GNU GPL версии 3, см. [LICENSE](LICENSE)
- **Приложение MAX**: собственная лицензия VK
