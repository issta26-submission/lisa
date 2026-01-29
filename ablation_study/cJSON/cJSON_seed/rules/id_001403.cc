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
//<ID> 1403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *ref_item = NULL;
    char *buffer = NULL;
    int buffer_len = 256;
    int arr_size = 0;
    cJSON_bool printed_ok = 0;
    const char *label_value = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("one");
    item2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "label", "demo");
    ref_item = cJSON_CreateString("referenced");
    cJSON_AddItemReferenceToObject(root, "ref", ref_item);

    // step 3: Operate / Validate
    buffer = (char *)cJSON_malloc(buffer_len);
    memset(buffer, 0, buffer_len);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buffer_len, 1);
    arr_size = cJSON_GetArraySize(arr);
    label_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    (void)label_value;
    (void)arr_size;
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}