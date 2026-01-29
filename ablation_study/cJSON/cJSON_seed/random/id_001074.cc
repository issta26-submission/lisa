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
//<ID> 1074
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
    cJSON *str = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    double extracted_number = 0.0;
    const char *extracted_string = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create a root object, a number and a string
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("hello world");

    // step 3: Configure - attach the number and string to the root object
    add_ok_num = cJSON_AddItemToObject(root, "answer", num);
    add_ok_str = cJSON_AddItemToObject(root, "greeting", str);

    // step 4: Operate - fetch items and extract their values, render JSON text
    fetched_num = cJSON_GetObjectItem(root, "answer");
    fetched_str = cJSON_GetObjectItem(root, "greeting");
    extracted_number = cJSON_GetNumberValue(fetched_num);
    extracted_string = cJSON_GetStringValue(fetched_str);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score from the results
    validation_score = (int)(root != (cJSON *)0) + (int)(num != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(extracted_number == 42.0) + (int)(extracted_string != (const char *)0) + (int)(add_ok_num) + (int)(add_ok_str);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}