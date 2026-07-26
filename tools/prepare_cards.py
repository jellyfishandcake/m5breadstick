#!/usr/bin/env python3
"""Prepare tarot card art for the firmware and generate a simple card catalog.

The script copies all PNG assets from an input folder into a processed assets
folder and a LittleFS data folder, then writes a small manifest and a C++ header
that the firmware can reference later.
"""

from __future__ import annotations

import argparse
import shutil
from pathlib import Path
from typing import Iterable

DEFAULT_ASSET_NAMES = ("card_back.png", "major_00_fool.png")


def prepare_output_dir(output_dir: Path) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    for child in output_dir.iterdir():
        if child.is_file():
            child.unlink()
        else:
            shutil.rmtree(child)


def copy_pngs(input_dir: Path, output_dir: Path, asset_names: Iterable[str] | None = None) -> list[Path]:
    prepare_output_dir(output_dir)
    copied: list[Path] = []
    names = list(asset_names or DEFAULT_ASSET_NAMES)
    for name in names:
        image_path = input_dir / name
        if image_path.is_file():
            target = output_dir / image_path.name
            shutil.copy2(image_path, target)
            copied.append(target)
    return copied


def write_manifest(output_dir: Path, copied_paths: Iterable[Path]) -> None:
    manifest = [path.name for path in copied_paths]
    (output_dir / "manifest.txt").write_text("\n".join(manifest) + "\n", encoding="utf-8")


def write_card_catalog(output_dir: Path, catalog_header: Path, copied_paths: Iterable[Path]) -> None:
    catalog_header.parent.mkdir(parents=True, exist_ok=True)
    names = [path.name for path in copied_paths]
    lines = [
        "#pragma once",
        "",
        "namespace card_catalog {",
        "static constexpr const char* kCardFiles[] = {",
    ]
    for name in names:
        lines.append(f'    "/cards/{name}",')
    lines.extend([
        "};",
        "static constexpr size_t kCardFileCount = sizeof(kCardFiles) / sizeof(kCardFiles[0]);",
        "}  // namespace card_catalog",
    ])
    catalog_header.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description="Prepare tarot card assets")
    parser.add_argument("input_dir", type=Path)
    parser.add_argument("output_dir", type=Path, nargs="?", default=Path("assets/cards/processed"))
    parser.add_argument("--data-dir", type=Path, default=Path("data/cards"))
    parser.add_argument("--catalog-header", type=Path, default=Path("src/card_catalog.h"))
    parser.add_argument("--asset-names", nargs="*", default=list(DEFAULT_ASSET_NAMES))
    args = parser.parse_args()

    input_dir = args.input_dir.resolve()
    output_dir = args.output_dir.resolve()
    data_dir = args.data_dir.resolve()
    catalog_header = args.catalog_header.resolve()

    copied_paths = copy_pngs(input_dir, output_dir, args.asset_names)
    if data_dir != output_dir:
        copy_pngs(input_dir, data_dir, args.asset_names)
    write_manifest(output_dir, copied_paths)
    write_card_catalog(output_dir, catalog_header, copied_paths)

    print(f"Prepared {len(copied_paths)} files into {output_dir}")
    print(f"Copied data files into {data_dir}")
    print(f"Wrote catalog header to {catalog_header}")


if __name__ == "__main__":
    main()
