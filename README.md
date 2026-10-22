<div align="center">
  <h1>MAX Flatpak (неофициальная сборка)</h1>

  [![Build Status](https://github.com/yrmuq/max-flatpak/actions/workflows/build.yml/badge.svg)](https://github.com/yrmuq/max-flatpak/actions/workflows/build.yml)
  [![Latest Release](https://img.shields.io/github/v/release/yrmuq/max-flatpak)](https://github.com/yrmuq/max-flatpak/releases/latest)
  [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

  <p>
    Неофициальная самодостаточная сборка мессенджера <code>MAX</code> в формате <code>Flatpak</code> на базе среды выполнения <code>org.freedesktop.Platform</code>, обеспечивающая изолированный запуск в песочнице без модификации хост-системы.
  </p>
</div>

---

> [!WARNING]
> **Неофициальная сборка:** Это **неофициальный** пакет мессенджера `MAX`, не связанный с разработчиками `VK` или создателями оригинального приложения. По вопросам работы самого мессенджера обращайтесь в официальную поддержку `VK` — данный репозиторий отвечает исключительно за упаковку.
>
> **Автономная упаковка:** Сборка целиком включает официальный `rpm`-пакет с `download.max.ru`. При установке на компьютер пользователя никакие дополнительные бинарные файлы из сети не загружаются.

---

## Особенности

* **Строгая изоляция файловой системы:** Прямой доступ ко всей домашней директории заблокирован; передача файлов осуществляется исключительно через системный диалог (`XDG Desktop Portal`).
* **Графический стек и мультимедиа:** Отрисовка окна выполняется через `X11` (или `XWayland` в сессиях `Wayland`, так как в официальном `rpm`-пакете отсутствует нативный плагин `Wayland`). Аудио- и видеозвонки работают через `PipeWire`, с поддержкой системных уведомлений и иконки в трее.
* **Автоматизированные обновления (`CI/CD`):** `GitHub Actions` ежедневно в `15:00 UTC` проверяет наличие нового `rpm`-пакета, автоматически собирает `MAX.flatpak`, подписывает репозиторий обновлений и публикует актуальный `ru.max.Unofficial.flatpakref` в разделе релизов.
* **Внутренняя структура:** Приложение размещается в `/app/opt/max` и запускается через исполняемый файл `/app/opt/max/bin/max` с идентификатором `ru.max.Unofficial`.

---

## Установка и использование

### Поддержка платформ и форматов

| Формат | `x86_64` | `arm64` |
| :--- | :---: | :---: |
| `Flatpak` | ✅ | ❌ |
| `AppImage` | ❌ | ❌ |

### Установка с автообновлением (`.flatpakref`)

Скачайте файл `ru.max.Unofficial.flatpakref` со страницы [Releases](https://github.com/yrmuq/max-flatpak/releases/latest) и выполните установку:

```bash
flatpak remote-delete --user max-repo || true
flatpak uninstall --user ru.max.Unofficial || true
flatpak install --user ru.max.Unofficial.flatpakref
flatpak run ru.max.Unofficial

```

Для последующего обновления пакета используйте стандартную команду:

```bash
flatpak update

```

### Автономная установка одним файлом (`.flatpak`)

Для установки без подключения репозитория автообновлений скачайте полный бандл `MAX.flatpak` со страницы [Releases](https://www.google.com/url?sa=E&source=gmail&q=https://github.com/yrmuq/max-flatpak/releases/latest):

```bash
flatpak uninstall --user ru.max.Unofficial || true
flatpak install --user MAX.flatpak
flatpak run ru.max.Unofficial

```

При выходе новой версии установка выполняется повторным запуском тех же команд поверх существующего пакета.

### Удаление

```bash
flatpak uninstall --user ru.max.Unofficial
flatpak remote-delete --user max-unofficial || true

```

---

## Лицензия и атрибуция

### Лицензия репозитория

Манифесты упаковки, конфигурационные файлы и скрипты автоматизации в этом репозитории распространяются под лицензией `GPL-3.0`. Подробности приведены в файле `LICENSE`.

### Сторонние компоненты

| Компонент | Upstream | Лицензия | Детали интеграции |
| --- | --- | --- | --- |
| `MAX` | [download.max.ru](https://www.google.com/search?q=https://download.max.ru/linux/rpm/el/9/x86_64/) | Проприетарная (`VK`) | Официальный `rpm`-пакет `x86_64`, распакованный в `/app/opt/max`. |
| `Freedesktop Platform` | [freedesktop-sdk/freedesktop-sdk](https://gitlab.com/freedesktop-sdk/freedesktop-sdk) | Различные открытые лицензии | Базовая среда выполнения `org.freedesktop.Platform`. |

