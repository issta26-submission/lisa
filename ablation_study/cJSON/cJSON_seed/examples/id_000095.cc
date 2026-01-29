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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(list, flag_true);
    cJSON_AddItemToArray(list, flag_false);
    cJSON *shared_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "shared", shared_num);
    cJSON_AddItemReferenceToArray(list, shared_num);

    // step 3: Operate & Validate
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    int count = cJSON_GetArraySize(got_list);
    cJSON *got_shared = cJSON_GetObjectItem(root, "shared");
    double shared_val = cJSON_GetNumberValue(got_shared);
    cJSON *summary = cJSON_CreateNumber(shared_val + (double)count);
    cJSON_AddItemToObject(root, "summary", summary);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}