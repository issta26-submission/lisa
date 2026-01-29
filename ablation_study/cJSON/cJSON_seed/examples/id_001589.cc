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
//<ID> 1589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char * strs[] = { "one", "two", "three" };
    cJSON *array = cJSON_CreateStringArray(strs, 3);
    cJSON_AddItemToObject(root, "strings", array);

    // step 2: Configure
    cJSON *refs = cJSON_CreateArray();
    cJSON *second_item = cJSON_GetArrayItem(array, 1);
    cJSON_AddItemReferenceToArray(refs, second_item);
    cJSON_AddItemToObject(root, "refs", refs);

    // step 3: Operate & Validate
    cJSON *has_strings_flag = cJSON_CreateNumber((double)cJSON_HasObjectItem(root, "strings"));
    cJSON_AddItemToObject(root, "has_strings", has_strings_flag);
    cJSON_DeleteItemFromArray(array, 1);
    int new_size = cJSON_GetArraySize(array);
    cJSON *size_num = cJSON_CreateNumber((double)new_size);
    cJSON_AddItemToObject(root, "strings_size", size_num);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}