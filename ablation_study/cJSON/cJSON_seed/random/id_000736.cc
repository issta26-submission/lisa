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
//<ID> 736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *tags = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *tag_a = (cJSON *)0;
    cJSON *tag_b = (cJSON *)0;
    cJSON *detached_name = (cJSON *)0;
    cJSON *retrieved_age = (cJSON *)0;
    cJSON *retrieved_tags = (cJSON *)0;
    cJSON *first_tag = (cJSON *)0;
    const char *detached_name_str = (const char *)0;
    const char *first_tag_str = (const char *)0;
    double age_value = 0.0;
    char *printed = (char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_tags = 0;
    cJSON_bool added_tag_a = 0;
    cJSON_bool added_tag_b = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    tags = cJSON_CreateArray();
    name_item = cJSON_CreateString("tester");
    age_item = cJSON_CreateNumber(27.0);
    tag_a = cJSON_CreateString("alpha");
    tag_b = cJSON_CreateString("beta");

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_age = cJSON_AddItemToObject(root, "age", age_item);
    added_tags = cJSON_AddItemToObject(root, "tags", tags);
    added_tag_a = cJSON_AddItemToArray(tags, tag_a);
    added_tag_b = cJSON_AddItemToArray(tags, tag_b);

    // step 4: Operate
    detached_name = cJSON_DetachItemFromObject(root, "name");
    detached_name_str = cJSON_GetStringValue(detached_name);
    retrieved_age = cJSON_GetObjectItem(root, "age");
    age_value = cJSON_GetNumberValue(retrieved_age);
    retrieved_tags = cJSON_GetObjectItem(root, "tags");
    first_tag = cJSON_GetArrayItem(retrieved_tags, 0);
    first_tag_str = cJSON_GetStringValue(first_tag);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (printed != (char *)0)
                     + (int)added_name + (int)added_age + (int)added_tags
                     + (int)added_tag_a + (int)added_tag_b
                     + (detached_name_str != (const char *)0)
                     + (first_tag_str != (const char *)0)
                     + (age_value > 0.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(detached_name);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}