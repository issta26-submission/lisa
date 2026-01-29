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
//<ID> 1530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *person = NULL;
    cJSON *age = NULL;
    cJSON *score = NULL;
    cJSON *age_dup = NULL;
    cJSON *age_sum = NULL;
    double age_val = 0.0;
    double age_copy_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    age = cJSON_CreateNumber(30.0);
    score = cJSON_CreateNumber(99.5);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(person, "score", score);
    cJSON_AddItemToObject(root, "person", person);

    // step 3: Operate / Validate
    age_dup = cJSON_Duplicate(age, 0);
    cJSON_AddItemToObject(root, "age_copy", age_dup);
    age_val = cJSON_GetNumberValue(age);
    age_copy_val = cJSON_GetNumberValue(age_dup);
    age_sum = cJSON_CreateNumber(age_val + age_copy_val);
    cJSON_AddItemToObject(root, "age_sum", age_sum);
    cJSON_DeleteItemFromObjectCaseSensitive(person, "score");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}