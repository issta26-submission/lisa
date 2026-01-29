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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *nameItem = NULL;
    cJSON *numItem = NULL;
    cJSON *gotNameItem = NULL;
    cJSON *gotNumItem = NULL;
    char *gotNameStr = NULL;
    double gotNumVal = 0.0;
    int validation = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    nameItem = cJSON_CreateString("tester");
    numItem = cJSON_CreateNumber(3.14);
    added1 = cJSON_AddItemToObject(root, "name", nameItem);
    added2 = cJSON_AddItemToObject(root, "value", numItem);

    // step 3: Operate
    gotNameItem = cJSON_GetObjectItem(root, "name");
    gotNumItem = cJSON_GetObjectItem(root, "value");
    gotNameStr = cJSON_GetStringValue(gotNameItem);
    gotNumVal = cJSON_GetNumberValue(gotNumItem);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(nameItem != NULL);
    validation ^= (int)(numItem != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(gotNameItem != NULL);
    validation ^= (int)(gotNumItem != NULL);
    validation ^= (int)(gotNameStr != NULL);
    validation ^= (int)((gotNameStr != NULL) && (gotNameStr[0] == 't'));
    validation ^= (int)(gotNumVal == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}