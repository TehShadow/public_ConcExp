

TEMPLATE_PATH = "./Helpers/makefile.start"
MAKE_PATH = "./Build/Makefiles/makefile.template"
CONFIG_PATH = "./config.conf"

def printConfig(PATH):
    f = open(PATH,"r")
    Lines = f.readlines()
    count = 0
    for line in Lines:
        count += 1
        line = line.strip()
        print(f"{count}) {line}")


def EditMakefile(PATH , replaced_content):
    replaced_make = ""
    f = open(PATH,"r")
    Lines = f.readlines()
    for line in Lines:
        if line.find("EndConfig") > 1:
            replaced_make = replaced_make + replaced_content + "\n"
        replaced_make = replaced_make + line
    f.close()

    write_file = open(MAKE_PATH, "w")
    write_file.write(replaced_make)
    write_file.close()


def EditConfig(PATH):
    replaced_content = ""
    f = open(PATH,"r")
    Lines = f.readlines()
    count = 0
    changeIndex = int(input("Choose line to edit >> "))
    for line in Lines:
        new_line = line.strip()
        count += 1
        if count == changeIndex:
            print(line)
            new_value = input("Insert new value >> ")
            old_value = new_line.split(" ")[2]
            new_line = new_line.replace(old_value,new_value)
        replaced_content = replaced_content + new_line + "\n"

    f.close()
    write_file = open(PATH, "w")
    write_file.write(replaced_content)
    write_file.close()
    EditMakefile(TEMPLATE_PATH,replaced_content)



if __name__ == "__main__":
    printConfig(CONFIG_PATH)
    EditConfig(CONFIG_PATH)