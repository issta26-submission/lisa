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
//<ID> 1471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"array\":[1,2],\"obj\":{\"a\":10,\"b\":20},\"remove\":123}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *array = cJSON_GetObjectItem(root, "array");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, newnum);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(array, obj_ref);
    cJSON *detached = cJSON_DetachItemFromObject(root, "remove");
    cJSON_AddItemToObject(obj, "detached", detached);
    cJSON *tmp = cJSON_CreateString("temp");
    cJSON_AddItemToObject(root, "tmp", tmp);
    cJSON_DeleteItemFromObject(root, "tmp");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON_bool has_array = cJSON_HasObjectItem(root, "array");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    (void)add_ref_ok;
    (void)first_is_num;
    (void)has_array;
    // API sequence test completed successfully
    return 66;
}