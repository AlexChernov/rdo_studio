import subprocess,os,argparse,re,sys

def run_bison(yxPath, yPath, cppPath, iswin):
    if iswin:
        procname = "set CYGWIN=nodosfilewarning& set BISON_PKGDATADIR=" + os.environ["BISON_PKGDATADIR"] + "&" + os.environ["BISON_PATH"] + " -g -pdpt \"" + yPath + "\" -o\"" + cppPath + "\""
    else:
        procname = "set BISON_PKGDATADIR=" + os.environ["BISON_PKGDATADIR"] + ";" + os.environ["BISON_PATH"] + " -g -pdpt \"" + yPath + "\" -o\"" + cppPath + "\""

    output, errr = subprocess.Popen(procname,stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = True).communicate()

    errr = errr.decode(codepage)

    errr = errr.replace(yPath,yxPath)
    
    if iswin:
        yPath = yPath.replace("\\","\\\\")   # ну это какой-то грязный хак, конечно
        yxPath = yxPath.replace("\\","\\\\")
        errr = errr.replace(yPath,yxPath)

    strGRAM = open(cppPath, encoding = codepage).read()

    fin = open(cppPath, 'w', encoding = codepage)
    fin.write( strGRAM.replace(yPath,yxPath) )
    fin.close()

    return re.sub(r"^([^:]*:)((\\\\?[^:\\]*)*):(([0-9]*).([0-9]*)-[0-9.]*)(.*)$", r"\1\2(\5,\6)\7", errr, flags=re.MULTILINE)

def main():
    parser = argparse.ArgumentParser(usage = argparse.SUPPRESS, description = "run bison twice for multipass compiler grammar files")

    parser.add_argument('inputFile', type = str, help = ".yx input file")

    parser.add_argument('-y1', type = str, default = '', help =\
                        "1st output y file", required = True)
    parser.add_argument('-y2', type = str, default = '', help =\
                        "2nd output y file", required = True)
    parser.add_argument('-o1', type = str, default = '', help =\
                        "1st output cpp file", required = True)
    parser.add_argument('-o2', type = str, default = '', help =\
                        "2nd output cpp file", required = True)

    parser.add_argument('-windows', action = "store_true", help =\
                        "win or linux")

    args = parser.parse_args()

    print(toolname + ": " + "Executing bison...")
    cppPath = os.path.abspath(args.o1)
    yPath   = os.path.abspath(args.y1)
    yxPath  = os.path.abspath(args.inputFile)
    print(toolname + ": " + "parsing " + yPath)
    
    out1 = run_bison(yxPath, yPath, cppPath, args.windows)

    cppPath = os.path.abspath(args.o2)
    yPath   = os.path.abspath(args.y2)
    yxPath  = os.path.abspath(args.inputFile)
    print(toolname + ": " + "parsing " + yPath)

    out2 = run_bison(yxPath, yPath, cppPath, args.windows)

    if out1 == out2:
        print(toolname + ": " + "bison output:")
        print(out1)
        sys.exit(0)
    else:
        sys.exit(yxPath + ": error : bison console output doesn't match for grammar files")

toolname = "run-bison"
codepage = "utf-8"

if __name__ == '__main__':
    main()
