#!/usr/bin/env python3
"""
Insert Doxygen-style documentation stubs into C/C++ source and header files.

Scans `src` and `lib` directories for files with extensions
.c, .cpp, .h, .hpp and inserts a file header (if missing) and a Doxygen
comment stub above each function definition or declaration it heuristically
detects. Original files are backed up with a .bak extension.

This script excludes any paths containing the folders `esp-nimble-cpp` or `Time`.

Usage: python tools/add_doc_comments.py
"""
import re
from pathlib import Path

ROOT = Path('.')
TARGET_DIRS = ['src', 'lib']
EXTS = ['.c', '.cpp', '.h', '.hpp']
EXCLUDE_KEYWORDS = ['esp-nimble-cpp', 'Time']

# Heuristic regexes for function defs and declarations (simple, line-based)
FUNC_DEF_RE = re.compile(r'^([\w\s:\<\>\*&\(\)\[\],~]+?)\b([A-Za-z_][A-Za-z0-9_:<>~]*)\s*\(([^;{}]*)\)\s*(const)?\s*\{\s*$')
FUNC_DECL_RE = re.compile(r'^([\w\s:\<\>\*&\(\)\[\],~]+?)\b([A-Za-z_][A-Za-z0-9_:<>~]*)\s*\(([^;{}]*)\)\s*(const)?\s*;\s*$')

SKIP_PATTERNS = re.compile(r'^(if|for|while|switch|catch|sizeof)\b')

def is_excluded(path: Path) -> bool:
    s = str(path).replace('\\', '/').lower()
    for k in EXCLUDE_KEYWORDS:
        if k.lower() in s:
            return True
    return False

def parse_params(param_str: str):
    params = []
    param_str = param_str.strip()
    if not param_str or param_str == 'void':
        return params
    depth = 0
    cur = ''
    for ch in param_str:
        if ch in '<([{':
            depth += 1
        elif ch in '>)]}':
            depth = max(0, depth-1)
        if ch == ',' and depth == 0:
            params.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip():
        params.append(cur.strip())
    parsed = []
    for p in params:
        parts = p.rsplit(' ', 1)
        name = parts[-1].strip()
        name = name.split('=')[0].strip()
        if name == '' or any(x in name for x in ['*', '&', '<', '(']):
            name = 'param'
        parsed.append((name, p))
    return parsed

def make_doc_stub(return_type: str, name: str, params_raw: str):
    params = parse_params(params_raw)
    lines = []
    lines.append('/**')
    lines.append(' * @brief Brief description of {}.'.format(name))
    lines.append(' *')
    if params:
        for pname, _ in params:
            lines.append(' * @param {} {}'.format(pname, 'Describe this parameter.'))
    rt = return_type.strip()
    if rt and rt != 'void' and not rt.endswith(')'):
        lines.append(' * @return {} {}'.format(rt, 'Describe the return value.'))
    lines.append(' *')
    lines.append(' * Algorithm:')
    lines.append(' * - Outline the high-level algorithm or approach used.')
    lines.append(' * - Mention important data structures or invariants.')
    lines.append(' *')
    lines.append(' * Loops:')
    lines.append(' * - Describe each loop purpose and termination condition.')
    lines.append(' * - Note whether loops are nested and their effect on complexity.')
    lines.append(' *')
    lines.append(' * Complexity:')
    lines.append(' * - Time: O(...)')
    lines.append(' * - Space: O(...)')
    lines.append(' */')
    return '\n'.join(lines) + '\n'

def process_file(path: Path):
    if is_excluded(path):
        print(f'Skipping excluded: {path}')
        return
    text = path.read_text(encoding='utf-8', errors='ignore')
    lines = text.splitlines()
    out_lines = []
    changed = False

    if not text.lstrip().startswith('/*') and 'Doxygen' not in text[:200]:
        header = [
            '/**',
            f' * @file {path.name}',
            ' * @brief Brief summary of this file.',
            ' *',
            ' * Detailed description of the file and its purpose.',
            ' */',
            ''
        ]
        out_lines.extend(header)
        changed = True

    i = 0
    total = len(lines)
    while i < total:
        line = lines[i]
        stripped = line.strip()
        m_def = FUNC_DEF_RE.match(stripped)
        m_decl = FUNC_DECL_RE.match(stripped)
        if m_def and not SKIP_PATTERNS.match(m_def.group(2)):
            return_type = m_def.group(1)
            func_name = m_def.group(2)
            params_raw = m_def.group(3)
            stub = make_doc_stub(return_type, func_name, params_raw)
            out_lines.append(stub.rstrip('\n'))
            out_lines.append(line)
            changed = True
            i += 1
            continue
        elif m_decl and not SKIP_PATTERNS.match(m_decl.group(2)):
            return_type = m_decl.group(1)
            func_name = m_decl.group(2)
            params_raw = m_decl.group(3)
            stub = make_doc_stub(return_type, func_name, params_raw)
            out_lines.append(stub.rstrip('\n'))
            out_lines.append(line)
            changed = True
            i += 1
            continue
        else:
            out_lines.append(line)
            i += 1

    if changed:
        bak = path.with_suffix(path.suffix + '.bak')
        try:
            path.rename(bak)
        except Exception:
            # if rename fails (e.g., backup already exists), overwrite backup
            bak.write_text(text, encoding='utf-8')
        path.write_text('\n'.join(out_lines) + '\n', encoding='utf-8')
        print(f'Updated: {path} (backup at {bak})')
    else:
        print(f'No changes: {path}')

def main():
    files = []
    for d in TARGET_DIRS:
        p = ROOT / d
        if not p.exists():
            continue
        for ext in EXTS:
            files.extend(sorted(p.rglob(f'*{ext}')))
    if not files:
        print('No target files found.')
        return
    for f in files:
        process_file(f)

if __name__ == '__main__':
    main()
