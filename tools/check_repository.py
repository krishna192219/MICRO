#!/usr/bin/env python3
"""Static distribution checks; no ROS installation or hardware required."""
import ast
import csv
import hashlib
import re
from pathlib import Path
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]


def main():
    errors = []
    python_files = list(ROOT.rglob('*.py'))
    for path in python_files:
        try:
            ast.parse(path.read_text(), filename=str(path))
        except SyntaxError as exc:
            errors.append(f'{path.relative_to(ROOT)}: {exc}')
    for path in ROOT.rglob('*.md'):
        text = path.read_text()
        if text.count('```') % 2:
            errors.append(f'Unbalanced fenced block: {path}')
        for link in re.findall(r'\]\(([^)]+)\)', text):
            if link.startswith(('https://', 'http://', '#', 'mailto:')):
                continue
            target = link.split('#', 1)[0]
            if target and not (path.parent / target).exists():
                errors.append(f'Broken link in {path.relative_to(ROOT)}: {target}')
    ET.parse(ROOT / 'ros2_ws/src/robot_mapping/package.xml')
    rows = list(csv.DictReader((ROOT / 'docs/source_manifest.csv').open()))
    available = {}
    for row in rows:
        p = ROOT / row['repository_path']
        digest = hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else ''
        if digest == row['original_sha256']:
            available[row['original_path']] = True
    for source in {row['original_path'] for row in rows}:
        if source not in available:
            errors.append(f'Original bytes not preserved anywhere: {source}')
    for path in ROOT.glob('firmware/*/*/*.ino'):
        if path.stem != path.parent.name:
            errors.append(f'Arduino sketch/folder mismatch: {path}')
    assert not errors, '\n'.join(errors)
    print(f'PASS: {len(python_files)} Python files parse; Markdown links/fences valid; '
          f'package XML valid; all {len(available)} unique source/assets preserved.')
    print('Not a ROS build, Arduino compilation, or hardware test.')


if __name__ == '__main__':
    main()
