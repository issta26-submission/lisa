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
//<ID> 479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retrieved_items = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *greet_item = NULL;
    int arr_size = 0;
    const char *greet_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(child, "greet", "hello");
    cJSON_AddItemToObject(root, "meta", child);
    cJSON_AddStringToObject(root, "title", "rootTitle");

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 64, 1);
    parsed = cJSON_Parse(printed);
    retrieved_items = cJSON_GetObjectItem(parsed, "items");
    arr_size = cJSON_GetArraySize(retrieved_items);
    retrieved_meta = cJSON_GetObjectItem(parsed, "meta");
    greet_item = cJSON_GetObjectItem(retrieved_meta, "greet");
    greet_str = cJSON_GetStringValue(greet_item);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';
    (void)greet_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}