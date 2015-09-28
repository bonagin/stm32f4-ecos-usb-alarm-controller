#ifndef SRC_F4_BACKUPDOMAIN_H_
#define SRC_F4_BACKUPDOMAIN_H_

class F4_BackupDomain
{
public:

	static cyg_bool powerBackup();

	static cyg_bool disableWRprotection();
	static cyg_bool enableWRprotection();
};

#endif /* SRC_F4_BACKUPDOMAIN_H_ */
