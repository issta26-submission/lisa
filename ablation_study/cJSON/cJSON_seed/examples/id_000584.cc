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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *people = cJSON_CreateArray();
    cJSON *alice = cJSON_CreateString("alice");
    cJSON *bob = cJSON_CreateString("bob");
    cJSON_AddItemToArray(people, alice);
    cJSON_AddItemToArray(people, bob);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "people", people);
    cJSON_AddStringToObject(root, "group", "testers");
    int count = cJSON_GetArraySize(people);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "owner", "owner1");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_people = cJSON_GetObjectItem(dup, "people");
    int dup_size = cJSON_GetArraySize(dup_people);
    char *snapshot = cJSON_PrintUnformatted(dup);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;dup_count=%d", version ? version : "null", count, dup_size);
    cJSON_free(snapshot);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}