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
//<ID> 1323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2],\"items\":[\"alpha\",\"beta\"]}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first_number = cJSON_GetArrayItem(numbers, 0);
    cJSON *new_number = cJSON_CreateNumber(3.14);
    cJSON_InsertItemInArray(numbers, 1, new_number);
    cJSON_SetNumberHelper(first_number, 9.81);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *orig_str = cJSON_CreateString("gamma");
    cJSON_AddItemToObject(root, "orig", orig_str);
    cJSON_AddItemReferenceToArray(items, orig_str);
    cJSON *tag = cJSON_CreateString("delta");
    cJSON_AddItemToObject(root, "tag", tag);

    // step 3: Operate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buffer);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, buffer, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(buf);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}