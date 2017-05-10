import sys
import json

# template
template = '''
void {type}Copy(corto_object *dst, corto_object src)
{
    if (src == nullptr)
    {
        return;
    }

    {type}{ptr} obj = {type}(src);
    {type}{ptr} *dest = ({type}{ptr}*)dst;

    if (*dest == nullptr)
    {
        *dest = {type}Create({members});
        {hidden_members}
    }
    else
    {
        {type}Assign(*dest, {members});
    }
}
'''

def ProcessTemplate(kmap, template, outfile):
    code = template;
    for k,v in kmap.items():
        code = code.replace("{"+k+"}", v);
    outfile.write(code);

def GenerateLoadCallbacks(scope, types, outfile):
    genFunc = "\nvoid Load{0}CopyCallbacks()\n".format(scope)
    genFunc += "{\n"
    for t in types:
        genFunc += "\tCORTO_OLS_SET_COPY_CB({0});\n".format(t)

    genFunc += "}\n"

    outfile.write(genFunc)


if __name__ == '__main__':
    argvLen = len(sys.argv)
    if argvLen < 2:
        print 'Usage = copy_gen.py <config.json>\n'
        print 'Argument List:', str(sys.argv)
        sys.exit(0)
    configFile = sys.argv[1]

    print 'copy_gen.py %s ' %(configFile)
    config = []
    types  = []
    items = []

    with open(configFile) as cfile:
        sconf = cfile.read()
        config = json.loads(sconf)

    items = config["items"]

    with open("src/copy_callbacks.cpp", "w") as sfile:
        sfile.write(config["includes"])
        for item in items:
            types.append(item["type"])
            ProcessTemplate(item, template, sfile)
        GenerateLoadCallbacks(config["scope"], types, sfile)
