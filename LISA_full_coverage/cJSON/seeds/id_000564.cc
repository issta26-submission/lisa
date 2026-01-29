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
//<ID> 564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"name\":\"orig\",\"items\":[\"one\",\"two\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, strlen(raw_json) + 1, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *new_item = cJSON_CreateString("three");
    cJSON_AddItemReferenceToArray(items, new_item);

    // step 3: Operate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "modified");
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddNumberToObject(root, "has_items", (double)has_items);
    size_t printed_len = printed ? strlen(printed) : 0;
    void *scratch = cJSON_malloc(printed_len + 32);
    memset(scratch, 0, printed_len + 32);
    strcpy((char *)scratch, printed);

    // step 4: Validate & Cleanup
    double extracted_flag = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "has_items"));
    (void)extracted_flag;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}