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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"person\":{\"name\":\"Bob\",\"age\":25},\"tags\":[\"x\",\"y\"]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *age = cJSON_GetObjectItem(person, "age");

    // step 2: Configure
    cJSON *new_age = cJSON_CreateNumber(30.0);
    cJSON_ReplaceItemViaPointer(person, age, new_age);
    cJSON *tags = cJSON_GetObjectItem(root, "tags");
    cJSON *extra_tag = cJSON_CreateString("z");
    cJSON_AddItemToArray(tags, extra_tag);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON *age_check = cJSON_GetObjectItem(person, "age");
    double age_val = cJSON_GetNumberValue(age_check);
    cJSON *age_echo = cJSON_CreateNumber(age_val);
    cJSON_AddItemToObject(root, "age_echo", age_echo);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}