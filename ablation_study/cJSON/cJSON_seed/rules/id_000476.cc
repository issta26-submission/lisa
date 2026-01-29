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
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON *item3 = cJSON_CreateNumber(3.14);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_items = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_label = NULL;
    char *label_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToArray(arr, item3);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(meta, "label", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 128, 1);
    parsed = cJSON_Parse(printed);
    retr_items = cJSON_GetObjectItem(parsed, "items");
    arr_size = cJSON_GetArraySize(retr_items);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_label = cJSON_GetObjectItem(retr_meta, "label");
    label_str = cJSON_GetStringValue(retr_label);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';
    buffer[2] = label_str[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}