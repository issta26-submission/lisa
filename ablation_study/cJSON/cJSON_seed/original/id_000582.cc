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
//<ID> 582
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"Alice\", \"age\": 30 }, \"tags\": [\"dev\",\"test\"], \"remove\": \"bye\" }";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    const char *original_name = cJSON_GetStringValue(name_item);
    char *old_name = cJSON_SetValuestring(name_item, "Bob");
    cJSON *dup_person = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_copy", dup_person);

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 128, 1);
    int prelen = 512;
    char *prebuf = (char *)cJSON_malloc(prelen);
    memset(prebuf, 0, prelen);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, prelen, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove");

    // step 4: Validate & Cleanup
    const char *copied_name = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "person_copy"), "name"));
    (void)original_name;
    (void)old_name;
    (void)pre_ok;
    (void)copied_name;
    cJSON_free(buffered);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}