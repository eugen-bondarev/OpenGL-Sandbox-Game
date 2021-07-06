
namespace mw {

std::string ROOT_DIR = "";

void DefRoot(int amountOfArguments, char *arguments[])
{
	std::string root = std::string(arguments[0]);
	int slash0 = root.find_last_of('\\');
	int slash1 = root.find_last_of('/');
	int slashIndex = (slash0 != std::string::npos ? slash0 : slash1);
	root = root.substr(0, slashIndex) + "/";
	
	ROOT_DIR = root;
}

}