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
//<ID> 825
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON *retrieved2 = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    char *printed = (char *)0;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool cmp_equal = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(42.5);
    num2 = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "valueA", num1);
    cJSON_AddItemToObject(root, "valueB", num2);

    // step 3: Operate
    retrieved1 = cJSON_GetObjectItem(root, "valueA");
    retrieved2 = cJSON_GetObjectItem(root, "valueB");
    val1 = cJSON_GetNumberValue(retrieved1);
    val2 = cJSON_GetNumberValue(retrieved2);
    dup_num = cJSON_Duplicate(retrieved1, 1);
    cmp_equal = cJSON_Compare(retrieved1, dup_num, 1);

    // step 4: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved1 != (cJSON *)0) + (retrieved2 != (cJSON *)0) + (dup_num != (cJSON *)0) + ((val1 == 42.5) ? 1 : 0) + ((val2 == 42.5) ? 1 : 0) + (int)cmp_equal + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 5: Cleanup
    cJSON_Delete(dup_num);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}