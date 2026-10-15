# MAX Flatpak (неофициальная сборка)

[![Build Status](https://github.com/yrmuq/max-package/actions/workflows/build.yml/badge.svg)](https://github.com/yrmuq/max-package/actions/workflows/build.yml)
[![Latest Release](https://img.shields.io/github/v/release/yrmuq/max-package)](https://github.com/yrmuq/max-package/releases/latest)

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

## Установка

Скачайте `MAX.flatpak` со страницы [Releases](https://github.com/yrmuq/max-package/releases/latest). Это полный пакет.

```bash
flatpak uninstall --user ru.max.Unofficial || true
flatpak install --user MAX.flatpak
flatpak run ru.max.Unofficial
```


---

## Обновление

Скачайте новый `MAX.flatpak` и поставьте его поверх:

```bash
flatpak install --user MAX.flatpak
```

---

## Удаление

```bash
flatpak uninstall --user ru.max.Unofficial
```

---

## Технические детали

- **App ID**: `ru.max.Unofficial`
- **Runtime**: `org.freedesktop.Platform 26.08`
- **Источник**: официальный rpm `https://download.max.ru/linux/rpm/el/9/x86_64/`
- Программа лежит в `/app/opt/max` и запускается как `/app/opt/max/bin/max`
- Домашняя папка закрыта. Есть сеть, звук, камера, PipeWire, уведомления и трей.
- GitHub Actions раз в 6 часов проверяет новый rpm. Если версия новая, собирает полный `MAX.flatpak` и кладёт его в Releases.

Вопросы о самом мессенджере — в поддержку VK. Этот репозиторий отвечает только за упаковку.

- **Упаковка и скрипты**: MIT, см. [LICENSE](LICENSE)
- **Приложение MAX**: собственная лицензия VK
