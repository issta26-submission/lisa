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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "[{\"name\":\"item1\"},{\"name\":\"item2\"}]";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *array = root;
    cJSON *insert_obj = cJSON_CreateObject();
    cJSON *insert_name = cJSON_CreateString("inserted");
    cJSON *first_item = NULL;
    cJSON *second_item = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int size = 0;
    cJSON_bool is_name_string = 0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(insert_obj, "name", insert_name);
    cJSON_InsertItemInArray(array, 1, insert_obj);
    cJSON_AddItemReferenceToArray(array, cJSON_GetArrayItem(array, 0));

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(array);
    first_item = cJSON_GetArrayItem(array, 0);
    second_item = cJSON_GetArrayItem(array, 1);
    is_name_string = cJSON_IsString(cJSON_GetObjectItem(first_item, "name"));
    cmp = cJSON_Compare(first_item, second_item, 0);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (size % 10));
    buffer[1] = (char)('0' + (int)is_name_string);
    buffer[2] = (char)('0' + (int)cmp);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}