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
//<ID> 433
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
    cJSON *list = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON *meta_val = cJSON_CreateNumber(7.0);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_list = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *retrieved_meta_value = NULL;
    cJSON *first_item = NULL;
    double meta_value = 0.0;
    double first_value = 0.0;
    int list_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(list, num1);
    cJSON_AddItemToArray(list, num2);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "value", meta_val);
    cJSON_AddStringToObject(root, "name", "sample");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_meta = cJSON_GetObjectItem(parsed, "meta");
    retrieved_meta_value = cJSON_GetObjectItem(retrieved_meta, "value");
    meta_value = cJSON_GetNumberValue(retrieved_meta_value);
    retrieved_list = cJSON_GetObjectItem(parsed, "list");
    list_size = cJSON_GetArraySize(retrieved_list);
    first_item = cJSON_GetArrayItem(retrieved_list, 0);
    first_value = cJSON_GetNumberValue(first_item);
    buffer[0] = (char)('0' + (list_size % 10));
    buffer[1] = (char)('A' + ((int)first_value % 26));
    buffer[2] = (char)('0' + ((int)meta_value % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}