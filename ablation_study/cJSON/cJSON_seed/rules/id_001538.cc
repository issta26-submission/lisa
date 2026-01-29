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
//<ID> 1538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *dup_child = NULL;
    cJSON *num_item = NULL;
    cJSON *retrieved = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *newnum = NULL;
    double val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", num_item);
    dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "orig", child);
    cJSON_AddItemToObject(root, "dup", dup_child);

    // step 3: Operate / Validate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "orig");
    retrieved = cJSON_GetObjectItem(root, "dup");
    retrieved_num = cJSON_GetObjectItem(retrieved, "value");
    val = cJSON_GetNumberValue(retrieved_num);
    newnum = cJSON_CreateNumber(val + 1.5);
    cJSON_AddItemToObject(root, "incremented", newnum);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}