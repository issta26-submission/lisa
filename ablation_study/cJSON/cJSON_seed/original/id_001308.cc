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
//<ID> 1308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON *ref_item = cJSON_CreateStringReference("reference_text");
    cJSON *array = cJSON_CreateArray();
    cJSON *elem0 = cJSON_CreateString("first_element");
    cJSON *elem1 = cJSON_CreateString("second_element");

    // step 2: Configure
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem1);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON_AddItemToObject(root, "reference", ref_item);
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate
    cJSON *items_obj = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first = cJSON_GetArrayItem(items_obj, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *copied = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "copied_first", copied);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}