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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON *first_item = NULL;
    cJSON *last_item = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    int last_index = 0;
    cJSON_bool insert_result = 0;
    cJSON_bool addref_result = 0;
    cJSON_bool is_str = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_AddItemToArray(array, s1);
    insert_result = cJSON_InsertItemInArray(array, 1, inserted);
    cJSON_AddItemToObject(root, "orig", s2);
    addref_result = cJSON_AddItemReferenceToArray(array, s2);

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(array, 0);
    arr_size = cJSON_GetArraySize(array);
    last_index = arr_size - 1;
    last_item = cJSON_GetArrayItem(array, last_index);
    is_str = cJSON_IsString(first_item);
    cmp_result = cJSON_Compare(last_item, cJSON_GetObjectItem(root, "orig"), 1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)is_str);
    buffer[1] = printed[0];
    buffer[2] = (char)('0' + (int)cmp_result);
    (void)insert_result;
    (void)addref_result;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}