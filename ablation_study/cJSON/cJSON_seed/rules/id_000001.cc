#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *trueItem = cJSON_CreateTrue();
    cJSON *rawItem = cJSON_CreateRaw("some:raw,value");
    cJSON *numItem = cJSON_CreateNumber(42.0);
    cJSON *strItem = cJSON_CreateString("example");

    // step 2: Configure (assemble object)
    cJSON_AddItemToObject(root, "boolKey", trueItem);
    cJSON_AddItemToObject(root, "rawKey", rawItem);
    cJSON_AddItemToObject(root, "numKey", numItem);
    cJSON_AddItemToObject(root, "strKey", strItem);
    cJSON *nullItem = cJSON_AddNullToObject(root, "nullKey");

    // step 3: Operate / Validate (use API checks and produce serialized output)
    char *serialized = cJSON_PrintUnformatted(root);
    cJSON_bool checkTrueDirect = cJSON_IsTrue(trueItem);
    cJSON *retrievedBool = cJSON_GetObjectItem(root, "boolKey");
    cJSON_bool checkTrueRetrieved = cJSON_IsTrue(retrievedBool);
    cJSON_bool checkRaw = cJSON_IsRaw(rawItem);
    const char *retrievedString = cJSON_GetStringValue(cJSON_GetObjectItem(root, "strKey"));
    double retrievedNumber = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "numKey"));
    (void)checkTrueDirect;
    (void)checkTrueRetrieved;
    (void)checkRaw;
    (void)retrievedString;
    (void)retrievedNumber;
    (void)nullItem;

    // step 4: Cleanup
    if (serialized) { cJSON_free(serialized); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}