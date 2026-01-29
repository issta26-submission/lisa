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
//<ID> 1076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToArray(items, num_item);
    cJSON *parsed = cJSON_Parse("{\"parsed_key\":\"parsed_value\",\"num\":42}");

    // step 2: Configure
    cJSON_AddItemReferenceToObject(root, "parsed_ref", parsed);
    cJSON_AddStringToObject(root, "title", "cJSON Test");

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = first_str[0];
    ((char *)scratch)[1] = (char)('0' + (count & 0xF));
    cJSON_free(printed);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}