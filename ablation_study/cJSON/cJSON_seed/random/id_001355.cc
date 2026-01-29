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
//<ID> 1355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *num_dup = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *str_dup = (cJSON *)0;
    cJSON *added_copy = (cJSON *)0;
    const char *sval = (const char *)0;
    double num_val = 0.0;
    double num_dup_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create object, a number and a string
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14);
    str = cJSON_CreateString("payload");

    // step 3: Configure - attach the number, duplicate the string and add a string copy using its value
    cJSON_AddItemToObject(root, "pi", num);
    str_dup = cJSON_Duplicate(str, 0);
    sval = cJSON_GetStringValue(str_dup);
    added_copy = cJSON_AddStringToObject(root, "copy", sval);
    num_dup = cJSON_Duplicate(num, 0);
    cJSON_AddItemToObject(root, "pi_copy", num_dup);

    // step 4: Operate - retrieve numeric values from the object and confirm string content via comparison
    num_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    num_dup_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi_copy"));

    // step 5: Validate - compute a simple score from observed values and a compare of the original and added string
    validation_score = (int)(num_val == 3.14) + (int)(num_dup_val == 3.14) + (int)cJSON_Compare(str, added_copy, 1);

    // step 6: Cleanup - delete root (frees attached items) and delete unattached duplicates
    cJSON_Delete(root);
    cJSON_Delete(str);
    cJSON_Delete(str_dup);

    // API sequence test completed successfully
    (void)validation_score;
    (void)sval;
    (void)added_copy;
    return 66;
}