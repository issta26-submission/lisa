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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *city_item = (cJSON *)0;
    cJSON *temp_item = (cJSON *)0;
    cJSON *location_str = (cJSON *)0;
    cJSON *adjusted_temp = (cJSON *)0;
    cJSON *got_loc = (cJSON *)0;
    cJSON *got_temp = (cJSON *)0;
    char json_buf[] = "  { \"city\" : \"Paris\" , \"temp\" : 20 }  ";
    double temp_val = 0.0;
    double adjusted_val = 0.0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;

    // step 2: Initialize
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);

    // step 3: Configure
    city_item = cJSON_GetObjectItem(parsed, "city");
    temp_item = cJSON_GetObjectItem(parsed, "temp");
    location_str = cJSON_CreateString(cJSON_GetStringValue(city_item));
    root = cJSON_CreateObject();
    add_ok1 = cJSON_AddItemToObject(root, "location", location_str);

    // step 4: Operate
    temp_val = cJSON_GetNumberValue(temp_item);
    adjusted_temp = cJSON_CreateNumber(0.0);
    adjusted_val = cJSON_SetNumberHelper(adjusted_temp, temp_val + 2.5);
    add_ok2 = cJSON_AddItemToObject(root, "adjusted_temp", adjusted_temp);

    // step 5: Validate
    got_loc = cJSON_GetObjectItem(root, "location");
    got_temp = cJSON_GetObjectItem(root, "adjusted_temp");

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}