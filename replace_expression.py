import os
import re
import argparse

def replace_expression_in_file(file_path, old_expr, new_expr, dry_run):
    """
    Replace occurrences of old_expr with new_expr in the given file.
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        # Regular expression to match old_expr bounded by whitespace, commas, line breaks, or special characters
        pattern = rf'(?<!\w){re.escape(old_expr)}(\b|\s|,|\.|\*|&|$)'
        modified_content = re.sub(pattern, lambda m: m.group(0).replace(old_expr, new_expr), content)

        if content != modified_content:
            if dry_run:
                print(f"[Dry Run] Would modify: {file_path}")
            else:
                with open(file_path, 'w', encoding='utf-8') as file:
                    file.write(modified_content)
                print(f"Modified: {file_path}")
    except Exception as e:
        print(f"Error processing {file_path}: {e}")

def rename_file(file_path, old_expr, new_expr, dry_run):
    """
    Rename a file if its name contains the old_expr, including special character suffixes.
    """
    dir_name, base_name = os.path.split(file_path)
    pattern = rf'(?<!\w){re.escape(old_expr)}(\b|\.|\*|&|$)'
    if re.search(pattern, base_name):
        new_base_name = re.sub(pattern, lambda m: m.group(0).replace(old_expr, new_expr), base_name)
        new_file_path = os.path.join(dir_name, new_base_name)
        if dry_run:
            print(f"[Dry Run] Would rename: {file_path} to {new_file_path}")
        else:
            os.rename(file_path, new_file_path)
            print(f"Renamed: {file_path} to {new_file_path}")
        return new_file_path
    return file_path

def process_directory(directory, extensions, old_expr, new_expr, dry_run):
    """
    Recursively traverse a directory to find and process files with specified extensions.
    """
    for root, _, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                file_path = os.path.join(root, file)
                file_path = rename_file(file_path, old_expr, new_expr, dry_run)
                replace_expression_in_file(file_path, old_expr, new_expr, dry_run)

def main():
    parser = argparse.ArgumentParser(description="Recursively replace expressions in files and rename files.")
    parser.add_argument("directory", help="Root directory to search for files.")
    parser.add_argument("old_expr", help="Expression to be replaced.")
    parser.add_argument("new_expr", help="Expression to replace with.")
    parser.add_argument("-e", "--extensions", nargs='+', default=[".cpp", ".h", ".rst", ".ino", ".txt"],
                        help="List of file extensions to process (default: .cpp, .h, .rst, .ino, .txt).")
    parser.add_argument("--dry-run", action="store_true", help="Run in dry mode (no changes will be made).")

    args = parser.parse_args()

    process_directory(args.directory, args.extensions, args.old_expr, args.new_expr, args.dry_run)

if __name__ == "__main__":
    main()
