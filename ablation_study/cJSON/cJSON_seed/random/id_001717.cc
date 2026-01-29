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
//<ID> 1717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *got_str = (cJSON *)0;
    char *json_unformatted = (char *)0;
    const char *extracted_string = (const char *)0;
    double extracted_number = 0.0;
    cJSON_bool added_value = 0;
    cJSON_bool added_note = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.0);
    str_item = cJSON_CreateString("testcase");

    // step 3: Configure
    added_value = cJSON_AddItemToObject(root, "value", num_item);
    added_note = cJSON_AddItemToObject(root, "note", str_item);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted((const cJSON *)root);
    got_num = cJSON_GetObjectItem((const cJSON * const)root, "value");
    got_str = cJSON_GetObjectItemCaseSensitive((const cJSON * const)root, "note");
    extracted_number = cJSON_GetNumberValue(got_num);
    extracted_string = cJSON_GetStringValue(got_str);

    // step 5: Validate
    validation = (int)(extracted_number == 123.0)
               + (int)(extracted_string != (const char *)0 && extracted_string[0] == 't')
               + (int)(json_unformatted != (char *)0)
               + (int)added_value
               + (int)added_note;
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)num_item;
    (void)str_item;
    (void)got_num;
    (void)got_str;
    (void)extracted_string;
    (void)extracted_number;
    (void)added_value;
    (void)added_note;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}