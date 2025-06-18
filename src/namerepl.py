import re
import sys

def update_header_file(header_file, class_name):
    with open(header_file, "r") as file:
        lines = file.readlines()

    updated_lines = []
    in_class = False
    for line in lines:
        if f"class {class_name}" in line:
            in_class = True

        if in_class and re.match(rf"\s*{class_name}\s*\(", line):
            # Check if engine is already added
            if "Engine& engine" not in line:
                # Modify constructor signatures
                line = re.sub(rf"({class_name}\s*\(.*?\))",
                              rf"\1, Engine& engine = Engine::primary()",
                              line)

        updated_lines.append(line)

    with open(header_file, "w") as file:
        file.writelines(updated_lines)

def update_cpp_file(source_file, class_name):
    with open(source_file, "r") as file:
        lines = file.readlines()

    updated_lines = []
    for line in lines:
        # Update constructor definition
        match = re.match(rf"({class_name}::\s*{class_name}\s*\(.*?\))", line)
        if match:
            if "Engine& engine" not in line:
                line = re.sub(rf"({class_name}::\s*{class_name}\s*\(.*?\))",
                              rf"\1, Engine& engine",
                              line)

        # Update initializer list if needed
        if f"{class_name}::" in line and "{" in line:
            updated_lines.append(line)
            indent = " " * (len(line) - len(line.lstrip()))
            updated_lines.append(f"{indent}: Unit(engine), PinUnit(pin, DIRECT), Smoothable()")
            continue

        updated_lines.append(line)

    with open(source_file, "w") as file:
        file.writelines(updated_lines)

def primary():
    if len(sys.argv) < 2:
        print("Usage: python update_class.py ClassName")
        sys.exit(1)

    class_name = sys.argv[1]
    header_file = f"{class_name}.h"
    source_file = f"{class_name}.cpp"

    update_header_file(header_file, class_name)
    update_cpp_file(source_file, class_name)
    print(f"Updated {header_file} and {source_file} successfully.")

if __name__ == "__main__":
    primary()
