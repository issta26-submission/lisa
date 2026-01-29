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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"arr\":[\"one\",\"two\"],\"num\":42}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *existing0 = cJSON_GetArrayItem(arr, 0);
    cJSON *new_str = cJSON_CreateString("three");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool insert_res = 0;
    cJSON_bool addref_res = 0;
    cJSON *item_at1 = NULL;
    cJSON_bool is_str = 0;
    cJSON_bool cmp = 0;
    const char *sval = NULL;

    // step 2: Setup / Configure
    insert_res = cJSON_InsertItemInArray(arr, 1, new_str);
    addref_res = cJSON_AddItemReferenceToArray(arr, existing0);

    // step 3: Operate / Validate
    item_at1 = cJSON_GetArrayItem(arr, 1);
    is_str = cJSON_IsString(item_at1);
    cmp = cJSON_Compare(item_at1, existing0, 1);
    printed = cJSON_PrintUnformatted(root);
    sval = cJSON_GetStringValue(item_at1);
    buffer[0] = (char)('0' + (int)is_str);
    buffer[1] = (char)('0' + (int)cmp);
    buffer[2] = sval ? sval[0] : 'x';
    buffer[3] = (char)('0' + (int)insert_res);
    buffer[4] = (char)('0' + (int)addref_res);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}