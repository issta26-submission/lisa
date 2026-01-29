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
//<ID> 566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\" : \"tester\", \"value\" : 13.5, \"flag\" : true, \"label\" : \"orig\" } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(mutable_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    cJSON *refs_arr = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs_arr, label_item);
    cJSON_AddItemToObject(root, "refs", refs_arr);
    char *new_label = cJSON_SetValuestring(label_item, "modified");
    (void)new_label;
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON_AddBoolToObject(root, "has_name", has_name);

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 128, 1);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    char *scratch = (char *)cJSON_malloc(snaplen + 32);
    memset(scratch, 0, snaplen + 32);
    if (snapshot) strcpy(scratch, snapshot);

    // step 4: Validate & Cleanup
    double extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)extracted_value;
    cJSON_free(snapshot);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}