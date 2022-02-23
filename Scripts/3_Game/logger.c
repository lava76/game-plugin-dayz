class GameLabsLogger {
    private static const string basePath = "$profile:@Logging";
    private string fullPath;

    private string modIdentifier;
    private FileHandle fh;

    private bool allowDebug;
    private bool logrotate;

    void GameLabsLogger(string modIdentifier, bool allowDebug = false, bool logrotate = true) {
        this.allowDebug = allowDebug;
        this.logrotate  = logrotate;

        this.modIdentifier = modIdentifier;

        this._Setup();
    }

    void OverrideDebugStatus(bool allowDebug) {
        this.allowDebug = allowDebug;
    }

    void Debug(string message) {
        if(!this.allowDebug) return;
        this.Write("DEBUG\t", message);
    }

    void Info(string message) {
        this.Write("INFO\t", message);
    }

    void Warn(string message) {
        this.Write("WARN\t", message);
    }

    void Error(string message) {
        this.Write("ERROR\t", message);
    }

    void Close() {
        delete this;
    }

    private void Write(string level, string message) {
        string logged = string.Format("%1 | [%2] %3", this.GetISO8601DT(), level, message);
        FPrintln(this.fh, logged);
        if(this.allowDebug) {
            Print(string.Format("[GameLabs-Debug] %1", logged));
        }
    }

    private string GetISO8601DT() {
        int s, mi, h, d, mo, y;

        GetHourMinuteSecondUTC(h, mi, s);
        GetYearMonthDayUTC(y, mo, d);

        return string.Format(
            "%1-%2-%3T%4:%5:%6+00:00",
            y.ToStringLen(2),
            mo.ToStringLen(2),
            d.ToStringLen(2),
            h.ToStringLen(2),
            mi.ToStringLen(2),
            s.ToStringLen(2),
        );
    }

    private void _Setup() {
        this.fullPath = this.basePath + "/" + this.modIdentifier + ".log";
        if(!FileExist(this.basePath)) MakeDirectory(basePath);

        if(this.logrotate && FileExist(this.fullPath)) {
            string pattern = this.fullPath + "*";
            string fileName;
            FileAttr fileAttributes;
            FindFileHandle fileSearch = FindFile(pattern, fileName, fileAttributes, FindFileFlags.ALL);

            int resultsCounter = 0;

            bool hasResults = true;
            while(hasResults) {
                resultsCounter++;
                hasResults = FindNextFile(fileSearch, fileName, fileAttributes);
            }
            CloseFindFile(fileSearch);

            string archive_name = this.fullPath + "." + resultsCounter;
            CopyFile(this.fullPath, archive_name);
            DeleteFile(this.fullPath);
        }

        this.fh = OpenFile(this.fullPath, FileMode.APPEND);

        this._WriteDTSeperator();
    }

    private void _WriteDTSeperator() {
        FPrintln(this.fh, string.Format("************************** %1 **************************", this.GetISO8601DT()));
    }
    private void _WriteClosingLine() {
        FPrintln(this.fh, string.Format("EOF @ %1", this.GetISO8601DT()));
    }
};