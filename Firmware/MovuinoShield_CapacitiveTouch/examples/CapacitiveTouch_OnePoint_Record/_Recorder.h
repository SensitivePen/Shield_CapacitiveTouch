#include <SPIFFS.h>

class MovuinoRecorder {
    private:
    String _dirPath = "/data";
    String _fileName;
    String _filePath;

    File file;
    char sep = ',';
    bool _initRow = false; // avoid pushdata on same row as columns definition
    int _nRow = 0;         // count recorded row data

    long unsigned _timeRecord0;
    bool _isRecording = false;

    bool isEditable = false;
    bool isReadable = false;
    bool formatted;

    void initRecordFile();

    public:
    MovuinoRecorder();
    ~MovuinoRecorder();

    void begin();

    void newRecord(String fileName_);
    void defineColumns(String cols_);
    void addRow();

    void pushData(unsigned int data_);
    void stop();
    bool isRecording();

    int getFileNumber();

    void addNewRecord();
    void readAllRecords();
    void readFile(String filePath_);
    void writeData();

    void formatSPIFFS();
    void printStateSPIFFS();
    void listDirectory();
};
