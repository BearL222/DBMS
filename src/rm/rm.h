
#ifndef _rm_h_
#define _rm_h_

#include <string>
#include <vector>

#include "../rbf/rbfm.h"
#include "../ix/ix.h"

using namespace std;

# define RM_EOF (-1)  // end of a scan operator

// RM_ScanIterator is an iterator to go through tuples
class RM_ScanIterator {
public:
  RM_ScanIterator() {};
  ~RM_ScanIterator() {};

  // "data" follows the same format as RelationManager::insertTuple()
  RC getNextTuple(RID &rid, void *data);
  RC close();
  RBFM_ScanIterator rbfm_ScanIterator;
};


// RM_IndexScanIterator is an iterator to go through index entries
class RM_IndexScanIterator {
 public:
  RM_IndexScanIterator() {};  	// Constructor
  ~RM_IndexScanIterator() {}; 	// Destructor

  // "key" follows the same format as in IndexManager::insertEntry()
  RC getNextEntry(RID &rid, void *key);  	// Get next matching entry
  RC close();             			// Terminate index scan
  IX_ScanIterator ix_ScanIterator;
};


// Relation Manager
class RelationManager
{
public:
  static RelationManager* instance();

  RC createCatalog();

  RC deleteCatalog();

  RC createTable(const string &tableName, const vector<Attribute> &attrs);

  RC deleteTable(const string &tableName);

  RC getAttributes(const string &tableName, vector<Attribute> &attrs);

  RC insertTuple(const string &tableName, const void *data, RID &rid);

  RC deleteTuple(const string &tableName, const RID &rid);

  RC updateTuple(const string &tableName, const void *data, const RID &rid);

  RC readTuple(const string &tableName, const RID &rid, void *data);

  // Print a tuple that is passed to this utility method.
  // The format is the same as printRecord().
  RC printTuple(const vector<Attribute> &attrs, const void *data);

  RC readAttribute(const string &tableName, const RID &rid, const string &attributeName, void *data);

  // Scan returns an iterator to allow the caller to go through the results one by one.
  // Do not store entire results in the scan iterator.
  RC scan(const string &tableName,
      const string &conditionAttribute,
      const CompOp compOp,                  // comparison type such as "<" and "="
      const void *value,                    // used in the comparison
      const vector<string> &attributeNames, // a list of projected attributes
      RM_ScanIterator &rm_ScanIterator);

  RC createIndex(const string &tableName, const string &attributeName);

  RC destroyIndex(const string &tableName, const string &attributeName);

  // indexScan returns an iterator to allow the caller to go through qualified entries in index
  RC indexScan(const string &tableName,
                        const string &attributeName,
                        const void *lowKey,
                        const void *highKey,
                        bool lowKeyInclusive,
                        bool highKeyInclusive,
                        RM_IndexScanIterator &rm_IndexScanIterator);

// Extra credit work (10 points)
  RC addAttribute(const string &tableName, const Attribute &attr);

  RC dropAttribute(const string &tableName, const string &attributeName);

  bool validTable(const string& tableName);
  void createRD4Tables(vector<Attribute>& recordDescriptor);
  void createRD4Columns(vector<Attribute>& recordDescriptor);
  void createData4Tables(int table_id, const string& table_name, const string& file_name, void *data);
  void createData4Columns(int table_id, const string& column_name, int column_type, int column_length, int column_position, void *data);
  void createRD(const string& tableName, vector<Attribute>& RD);
  RC insertTupleNoCheck(FileHandle& fh, const string& tableName, const void* data1, RID& rid);
  RC updateTupleNoCheck(FileHandle& fh, const string& tableName, const void* data2, const RID& rid);
  RC deleteTupleNoCheck(FileHandle& fh, const string& tableName, const RID& rid);
  void createRD_id(vector<string>& RD_id);
  void setZero(RID& rid);
  void createRD_column(vector<string>& RD_column);
  RC insertAttr(const void* data, vector<Attribute>& attrs);


  void createIndexName(string& indexName, const string &tableName, const string &attributeName);
  bool hasIndex(RelationManager rm, const string& tableName, const string& attrName);

  RC insertInIndex(const string &tableName, const void *data, const RID &rid);
  RC deleteInIndex(const string& tableName, const void* data, const RID& rid);


  RelationManager();
  ~RelationManager();

private:
    static RelationManager* _rm;
    RecordBasedFileManager rbfm;
    IndexManager ixm;
};

#endif
