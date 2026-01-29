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
//<ID> 882
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddNumberToObject(info, "version", 2.5);

    // step 2: Configure
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON_AddNumberToObject(metadata, "answer", 42.0);
    cJSON_AddItemReferenceToObject(metadata, "name_ref", name);

    // step 3: Operate and Validate
    cJSON *fetched_name = cJSON_GetObjectItem(info, "name");
    char *name_str = (char*)cJSON_GetStringValue(fetched_name);
    double name_len = (double)strlen(name_str);
    cJSON_AddNumberToObject(root, "name_length", name_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}