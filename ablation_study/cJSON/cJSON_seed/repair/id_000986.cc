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
//<ID> 986
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
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, first);
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, second);
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(list, num);
    double newnum = cJSON_SetNumberHelper(num, 42.0);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *repl = cJSON_CreateString("replaced_beta");
    cJSON_ReplaceItemInArray(list, 1, repl);
    char *first_val = cJSON_GetStringValue(first);
    cJSON *copied_first = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "copied_first", copied_first);
    cJSON *num_dup = cJSON_CreateNumber(newnum);
    cJSON_AddItemToObject(root, "answer", num_dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}