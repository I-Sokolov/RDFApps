#pragma once

extern int IDSRun(std::string const& idsFile, std::string const& ifcFile, bool stopOnError, RDF::IDS::MsgLevel level);

extern int IDSRun_dir(const char* dir, bool stopOnError, RDF::IDS::MsgLevel level);
