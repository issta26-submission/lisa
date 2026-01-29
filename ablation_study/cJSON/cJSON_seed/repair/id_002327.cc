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
//<ID> 2327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entry = cJSON_CreateObject();
    const char *ref_literal = "reference-text";
    cJSON *ref = cJSON_CreateStringReference(ref_literal);
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(entry, "ref", ref);
    cJSON_AddItemToObject(entry, "num", num);
    cJSON_AddItemToObject(root, "entry", entry);

    // step 2: Configure
    cJSON *entry_copy = cJSON_Duplicate(entry, 1);
    cJSON_AddItemToObject(root, "entry_copy", entry_copy);

    // step 3: Operate
    char *json = cJSON_PrintUnformatted(root);
    cJSON *fetched_entry = cJSON_GetObjectItem(root, "entry");
    cJSON *fetched_ref = cJSON_GetObjectItem(fetched_entry, "ref");
    const char *ref_val = cJSON_GetStringValue(fetched_ref);
    cJSON *fetched_copy = cJSON_GetObjectItem(root, "entry_copy");
    cJSON *fetched_num = cJSON_GetObjectItem(fetched_copy, "num");
    double num_val = cJSON_GetNumberValue(fetched_num);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (ref_val && ref_val[0]) ? ref_val[0] : '\0';
    buffer[2] = (char)('0' + ((int)num_val % 10));
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}