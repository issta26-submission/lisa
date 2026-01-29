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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("device-list");
    int ids_vals[] = {101, 102, 103, 104};
    cJSON *ids_array = cJSON_CreateIntArray(ids_vals, 4);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "title", title);
    cJSON_AddItemToObjectCS(root, "data", data);
    cJSON_AddItemToObjectCS(data, "ids", ids_array);

    // step 3: Operate & Validate
    cJSON_bool has_title = cJSON_HasObjectItem(root, "title");
    (void)has_title;
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    char *title_str = (char *)cJSON_GetStringValue(got_title);
    cJSON *title_copy = cJSON_CreateString(title_str);
    cJSON_AddItemToObjectCS(root, "title_copy", title_copy);
    int ids_count = cJSON_GetArraySize(ids_array);
    (void)ids_count;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}