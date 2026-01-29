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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *letters = (cJSON *)0;
    cJSON *s_alpha = (cJSON *)0;
    cJSON *s_beta = (cJSON *)0;
    cJSON *raw_added = (cJSON *)0;
    cJSON *fetched_letters = (cJSON *)0;
    cJSON *fetched_alpha = (cJSON *)0;
    cJSON *fetched_beta = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array of strings
    root = cJSON_CreateObject();
    letters = cJSON_CreateArray();
    s_alpha = cJSON_CreateString("alpha");
    s_beta = cJSON_CreateString("beta");

    // step 3: Configure - populate array and attach to root, add raw JSON to root
    cJSON_AddItemToArray(letters, s_alpha);
    cJSON_AddItemToArray(letters, s_beta);
    cJSON_AddItemToObject(root, "letters", letters);
    raw_added = cJSON_AddRawToObject(root, "raw_data", "{\"num\":42,\"ok\":true}");

    // step 4: Operate - retrieve array and its elements, check types
    fetched_letters = cJSON_GetObjectItem(root, "letters");
    fetched_alpha = cJSON_GetArrayItem(fetched_letters, 0);
    fetched_beta = cJSON_GetArrayItem(fetched_letters, 1);
    cJSON_IsString(fetched_alpha);
    cJSON_IsString(fetched_beta);

    // step 5: Validate - serialize and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(fetched_letters != (cJSON *)0) + (int)cJSON_IsString(fetched_alpha) + (int)cJSON_IsString(fetched_beta) + (int)(raw_added != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}