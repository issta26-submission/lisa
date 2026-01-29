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
//<ID> 1135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *ref_text = "my_ref";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("one");
    cJSON *item_b = cJSON_CreateString("two");
    cJSON *item_c = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToArray(arr, item_c);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *ref = cJSON_CreateStringReference(ref_text);
    cJSON_AddItemToObject(root, "ref", ref);
    cJSON *truth = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", truth);
    cJSON *nul = cJSON_AddNullToObject(root, "nullable");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate
    cJSON *mid = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_str = cJSON_IsString(mid);
    double was_true = (double)cJSON_IsTrue(truth);
    double computed = (double)cJSON_GetArraySize(arr) + (double)is_str + was_true;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}