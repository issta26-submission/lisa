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
//<ID> 864
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14);
    const char *name_literal = "example_name";
    cJSON *name_ref = cJSON_CreateStringReference(name_literal);
    cJSON *meta = cJSON_CreateObject();
    cJSON *version = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "number", num_item);
    cJSON_AddItemToObjectCS(root, "name_ref", name_ref);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(meta, "version", version);

    // step 3: Operate & Validate
    double new_version = cJSON_SetNumberHelper(version, 2.0);
    cJSON *derived = cJSON_CreateNumber(new_version + 0.5);
    cJSON_AddItemToObjectCS(root, "derived", derived);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}