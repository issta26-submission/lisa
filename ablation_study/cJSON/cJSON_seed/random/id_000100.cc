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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *entry = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON *retrieved_entry = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    int list_size = 0;
    cJSON *detached = (cJSON *)0;
    cJSON *replacement = cJSON_CreateString("replaced_entry");
    cJSON *final_ptr = (cJSON *)0;

    // step 2: Setup - assemble objects and array
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(entry, "list", arr);
    cJSON_AddItemToObject(root, "entry", entry);

    // step 3: Operate - inspect array size, record it, detach and prepare replacement
    retrieved_entry = cJSON_GetObjectItem(root, "entry");
    retrieved_list = cJSON_GetObjectItem(retrieved_entry, "list");
    list_size = cJSON_GetArraySize(retrieved_list);
    cJSON_AddNumberToObject(root, "list_size", (double)list_size);
    detached = cJSON_DetachItemFromObject(root, "entry");
    cJSON_AddItemToObject(root, "entry_backup", detached);
    cJSON_ReplaceItemInObject(root, "entry_backup", replacement);

    // step 4: Validate - retrieve the replaced item
    final_ptr = cJSON_GetObjectItem(root, "entry_backup");

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}