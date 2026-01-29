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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "person", person);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person, "age", age);
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON *s1 = cJSON_CreateNumber(85.0);
    cJSON_AddItemToArray(scores, s1);
    cJSON *s2 = cJSON_CreateNumber(92.0);
    cJSON_AddItemToArray(scores, s2);

    // step 2: Configure
    cJSON *got_name = cJSON_GetObjectItem(person, "name");
    char *set_ret = cJSON_SetValuestring(got_name, "Bob");
    cJSON *dup_person = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_backup", dup_person);

    // step 3: Operate and Validate
    cJSON *got_scores = cJSON_GetObjectItem(person, "scores");
    int score_count = cJSON_GetArraySize(got_scores);
    cJSON_AddNumberToObject(root, "score_count", (double)score_count);
    cJSON *dup_name = cJSON_GetObjectItem(dup_person, "name");
    const char *dup_name_str = cJSON_GetStringValue(dup_name);
    double first_code = (double)(unsigned char)dup_name_str[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_code);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}