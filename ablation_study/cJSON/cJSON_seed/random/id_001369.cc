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
//<ID> 1369
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
    cJSON *numItem = NULL;
    cJSON *strItem = NULL;
    cJSON *gotNum = NULL;
    cJSON *gotStr = NULL;
    char *printed = NULL;
    int validation = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    double extracted_num = 0.0;
    const char *extracted_str = NULL;

    // step 2: Setup
    root = cJSON_CreateObject();
    numItem = cJSON_CreateNumber(3.14);
    strItem = cJSON_CreateString("pi");
    added_num = cJSON_AddItemToObject(root, "number", numItem);
    added_str = cJSON_AddItemToObject(root, "text", strItem);

    // step 3: Operate
    printed = cJSON_PrintUnformatted(root);
    gotNum = cJSON_GetObjectItem(root, "number");
    gotStr = cJSON_GetObjectItem(root, "text");
    extracted_num = cJSON_GetNumberValue(gotNum);
    extracted_str = cJSON_GetStringValue(gotStr);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(numItem != NULL);
    validation ^= (int)(strItem != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(gotNum != NULL);
    validation ^= (int)(gotStr != NULL);
    validation ^= (int)(extracted_num == 3.14);
    validation ^= (int)(extracted_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}