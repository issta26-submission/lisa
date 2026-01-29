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
//<ID> 1410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed_min = NULL;
    cJSON *parsed_len = NULL;
    cJSON *wrapper = NULL;
    cJSON *num_pi = NULL;
    char minifiable[] = "  { \"alpha\": true , \"value\": 123 }  ";
    const char raw[] = "{\"beta\": [10,20]}";
    char *printed = NULL;
    size_t raw_len = sizeof(raw) - 1;

    // step 2: Setup / Configure
    cJSON_Minify(minifiable);
    parsed_min = cJSON_Parse(minifiable);
    parsed_len = cJSON_ParseWithLength(raw, raw_len);
    wrapper = cJSON_CreateObject();
    cJSON_AddItemToObject(wrapper, "from_min", parsed_min);
    cJSON_AddItemToObject(wrapper, "from_len", parsed_len);
    num_pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(wrapper, "pi", num_pi);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(wrapper, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(wrapper);
    // API sequence test completed successfully
    return 66;
}