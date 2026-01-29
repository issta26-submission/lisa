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
//<ID> 711
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *raw_copy = (char *)cJSON_malloc(256);
    memset(raw_copy, 0, 256);
    char *printed = (char *)cJSON_malloc(256);
    memset(printed, 0, 256);
    const char *json = "{\"outer\":{\"inner\":{\"num\":42.5,\"flag\":true},\"text\":\"hello\"}}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *root_outer = cJSON_GetObjectItem(parsed, "outer");
    cJSON *inner = cJSON_GetObjectItem(root_outer, "inner");
    cJSON *num_item = cJSON_GetObjectItem(inner, "num");
    cJSON *flag_item = cJSON_GetObjectItem(inner, "flag");
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON_bool flag_val = cJSON_IsTrue(flag_item);
    memcpy(raw_copy, json, strlen(json));
    raw_copy[strlen(json)] = '\0';

    // step 2: Setup / Configure
    cJSON *replica = cJSON_CreateObject();
    cJSON *replica_outer = cJSON_AddObjectToObject(replica, "outer");
    cJSON *replica_inner = cJSON_AddObjectToObject(replica_outer, "inner");
    cJSON_AddNumberToObject(replica_inner, "num_copy", num_val);
    cJSON_AddBoolToObject(replica_inner, "flag_copy", flag_val);
    cJSON_AddStringToObject(replica_outer, "text_copy", "hello");

    // step 3: Operate / Validate
    char *printed_unfmt = cJSON_PrintUnformatted(replica);
    size_t printed_len = printed_unfmt ? strlen(printed_unfmt) : 0;
    memcpy(printed, printed_unfmt ? printed_unfmt : "", printed_len);
    printed[printed_len] = '\0';
    cJSON_free(printed_unfmt);

    // step 4: Cleanup
    cJSON_free(raw_copy);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(replica);

    // API sequence test completed successfully
    return 66;
}