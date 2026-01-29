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
//<ID> 883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *author = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(config, "author", author);
    cJSON *shared_label = cJSON_CreateString("shared_label");
    cJSON_AddItemToObject(root, "label", shared_label);

    // step 2: Configure
    cJSON *module = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "module", module);
    cJSON_AddNumberToObject(module, "count", 5.0);
    cJSON_AddNumberToObject(root, "version", 1.2);
    cJSON_AddItemReferenceToObject(config, "label_ref", shared_label);

    // step 3: Operate and Validate
    cJSON *author_item = cJSON_GetObjectItem(config, "author");
    char *author_name = cJSON_GetStringValue(author_item);
    cJSON *meta_ref = cJSON_CreateStringReference(author_name);
    cJSON_AddItemToObject(root, "meta", meta_ref);
    cJSON *count_item = cJSON_GetObjectItem(module, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "copied_count", count_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}